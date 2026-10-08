import React from 'react';

export function PropertyTable({children}) {
  return (
    <table>
      <thead>
        <tr>
          <th>Property</th>
          <th>Type</th>
          <th>Description</th>
        </tr>
      </thead>
      <tbody>{children}</tbody>
    </table>
  );
}

export function PropertyEntry({property, type, children}) {
  return (
    <tr>
      <td><code>{property}</code></td>
      <td>{type}</td>
      <td>{children}</td>
    </tr>
  );
}
